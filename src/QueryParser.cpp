#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <tuple>
#include <stdexcept>
#include <iostream>

using namespace std;

// Structure to represent a query
struct Query {
    string type;          // SELECT, INSERT, etc.
    string table;        // Table name
    vector<string> columns; // List of columns to select
    vector<tuple<string, string, string>> conditions; // (column, operator, value)
};

// QueryParser class definition
class QueryParser {
public:
    Query parse(const string& query) {
        Query parsedQuery;
        size_t pos = 0;
        // Trim whitespace
        string q = trim(query);
        // Identify the query type
        size_t spacePos = q.find(' ');
        if (spacePos != string::npos) {
            parsedQuery.type = q.substr(0, spacePos);
            pos = spacePos + 1;
        }
        // Process the rest of the query
        while (pos < q.length()) {
            // Skip whitespace
            while (pos < q.length() && isspace(q[pos])) pos++;
            if (pos >= q.length()) break;
            // Check for FROM keyword
            if (q.substr(pos, 4) == "FROM") {
                pos += 4;
                while (pos < q.length() && isspace(q[pos])) pos++;
                size_t nextSpace = q.find(' ', pos);
                if (nextSpace == string::npos) {
                    parsedQuery.table = q.substr(pos);
                    pos = nextSpace;
                } else {
                    parsedQuery.table = q.substr(pos, nextSpace - pos);
                    pos = nextSpace;
                }
                continue;
            }
            // Check for SELECT keyword
            if (q.substr(pos, 6) == "SELECT") {
                pos += 6;
                while (pos < q.length() && isspace(q[pos])) pos++;
                size_t fromPos = q.find("FROM", pos);
                if (fromPos == string::npos) {
                    // Error: No FROM clause
                    break;
                }
                string columnsStr = q.substr(pos, fromPos - pos);
                parsedQuery.columns = splitColumns(columnsStr);
                pos = fromPos + 4; // Move past "FROM"
                continue;
            }
            // Check for WHERE clause
            if (q.substr(pos, 5) == "WHERE") {
                pos += 5;
                while (pos < q.length() && isspace(q[pos])) pos++;
                string conditionStr = q.substr(pos);
                size_t semicolonPos = conditionStr.find(';');
                if (semicolonPos != string::npos) {
                    conditionStr = conditionStr.substr(0, semicolonPos);
                }
                parsedQuery.conditions = parseConditions(conditionStr);
                pos = q.length(); // End of query
                continue;
            }
            // If none of the above, treat as unexpected input
            throw invalid_argument("Unexpected input in query: " + q.substr(pos));
        }
        return parsedQuery;
    }
private:
    vector<string> splitColumns(const string& columnsStr) {
        vector<string> columns;
        stringstream ss(columnsStr);
        string column;
        while (getline(ss, column, ',')) {
            columns.push_back(trim(column));
        }
        return columns;
    }
    vector<tuple<string, string, string>> parseConditions(const string& conditionStr) {
        vector<tuple<string, string, string>> conditions;
        // Simple condition parsing (column OP value)
        size_t andPos = conditionStr.find(" AND ");
        if (andPos != string::npos) {
            string cond1 = conditionStr.substr(0, andPos);
            string cond2 = conditionStr.substr(andPos + 5);
            vector<tuple<string, string, string>> temp;
            temp.push_back(parseCondition(cond1));
            temp.push_back(parseCondition(cond2));
            conditions.insert(conditions.end(), temp.begin(), temp.end());
        } else {
            conditions.push_back(parseCondition(conditionStr));
        }
        return conditions;
    }
    tuple<string, string, string> parseCondition(const string& condition) {
        size_t opPos = condition.find("=");
        if (opPos != string::npos) {
            string column = trim(condition.substr(0, opPos));
            string op = "=";
            string value = trim(condition.substr(opPos + 1));
            return make_tuple(column, op, value);
        }
        // Add more operators as needed (>, <, !=, etc.)
        throw invalid_argument("Unsupported operator in condition: " + condition);
    }
    string trim(const string& s) {
        size_t start = s.find_first_not_of(" \t");
        size_t end = s.find_last_not_of(" \t");
        if (start == string::npos) return "";
        return s.substr(start, end - start + 1);
    }
};

int main() {
    QueryParser parser;
    string query = "SELECT id, name FROM users WHERE age = 25;";
    Query parsed = parser.parse(query);
    std::cout << "type: " << parsed.type << std::endl;
    std::cout << "table: " << parsed.table << std::endl;
    std::cout << "columns: ";
    for (const auto& col : parsed.columns) std::cout << col << " ";
    std::cout << std::endl;
    std::cout << "conditions: ";
    for (const auto& cond : parsed.conditions) {
        std::cout << "(" << std::get<0>(cond) << ", " << std::get<1>(cond) << ", " << std::get<2>(cond) << ") ";
    }
    std::cout << std::endl;
    return 0;
} 