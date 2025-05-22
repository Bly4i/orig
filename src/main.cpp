#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <stdexcept>

class DatabaseException : public std::runtime_error {
public:
    explicit DatabaseException(const std::string& message)
        : std::runtime_error("Database error: " + message) {}
};

class ClientNotFoundException : public std::runtime_error {
public:
    explicit ClientNotFoundException(const std::string& message)
        : std::runtime_error("Client not found: " + message) {}
};

class ClientManager {
public:
    ClientManager(const std::string& connection_string)
        : connection_(connection_string), transaction_(connection_) {
    }

    void createDatabaseStructure() {
        transaction_.exec(R"(
            CREATE TABLE IF NOT EXISTS clients (
                id SERIAL PRIMARY KEY,
                first_name VARCHAR(255) NOT NULL,
                last_name VARCHAR(255) NOT NULL,
                email VARCHAR(255) UNIQUE NOT NULL
            );
        )");

        transaction_.exec(R"(
            CREATE TABLE IF NOT EXISTS phones (
                id SERIAL PRIMARY KEY,
                client_id INTEGER REFERENCES clients(id) ON DELETE CASCADE,
                phone_number VARCHAR(20)
            );
        )");

        transaction_.commit();
    }

    int addClient(const std::string& first_name, const std::string& last_name, const std::string& email) {
        pqxx::work txn(connection_);
        pqxx::result res = txn.exec_params(
            "INSERT INTO clients (first_name, last_name, email) VALUES ($1, $2, $3) RETURNING id",
            first_name, last_name, email
        );
        int client_id = res[0][0].as<int>();
        txn.commit();
        return client_id;
    }

    void addPhone(int client_id, const std::string& phone_number) {
        pqxx::work txn(connection_);
        txn.exec_params(
            "INSERT INTO phones (client_id, phone_number) VALUES ($1, $2)",
            client_id, phone_number
        );
        txn.commit();
    }

    void updateClient(int client_id, const std::string& first_name, const std::string& last_name, const std::string& email) {
        pqxx::work txn(connection_);
        txn.exec_params(
            "UPDATE clients SET first_name = $1, last_name = $2, email = $3 WHERE id = $4",
            first_name, last_name, email, client_id
        );
        txn.commit();
    }

    void deletePhone(int phone_id) {
        pqxx::work txn(connection_);
        txn.exec_params("DELETE FROM phones WHERE id = $1", phone_id);
        txn.commit();
    }

    void deleteClient(int client_id) {
        pqxx::work txn(connection_);
        txn.exec_params("DELETE FROM clients WHERE id = $1", client_id);
        txn.commit();
    }

    std::vector<std::tuple<int, std::string, std::string, std::string>> findClient(const std::string& search_term) {
        std::vector<std::tuple<int, std::string, std::string, std::string>> results;
        pqxx::work txn(connection_);
        pqxx::result res = txn.exec_params(
            R"(
                SELECT id, first_name, last_name, email
                FROM clients
                WHERE first_name ILIKE $1 OR last_name ILIKE $1 OR email ILIKE $1
            )",
            "%" + search_term + "%"
        );

        for (const auto& row : res) {
            results.emplace_back(
                row["id"].as<int>(),
                row["first_name"].as<std::string>(),
                row["last_name"].as<std::string>(),
                row["email"].as<std::string>()
            );
        }
        txn.commit();
        if (results.empty()) {
            throw ClientNotFoundException("No clients found for search term: " + search_term);
        }
        return results;
    }

private:
    pqxx::connection connection_;
    pqxx::work transaction_;
};

int main() {
    try {
        ClientManager manager("dbname=postgres user=postgres password=kima123 host=localhost port=5432");

        manager.createDatabaseStructure();

        int client_id = manager.addClient("John", "Doe", "john.doe@example.com");
        manager.addPhone(client_id, "+123456789");
        manager.addPhone(client_id, "+987654321");

        auto clients = manager.findClient("John");
        for (const auto& [id, first_name, last_name, email] : clients) {
            std::cout << "Found client: " << first_name << " " << last_name << " (" << email << ")" << std::endl;
        }

    }
    catch (const DatabaseException& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
    catch (const ClientNotFoundException& e) {
        std::cerr << "Client not found: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }

    return 0;
}
