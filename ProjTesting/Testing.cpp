#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <thread>
#include <cpr/cpr.h>

// Helper function to print response details
void printResponseDetails(const cpr::Response& r, const std::string& request_name) {
    std::cout << "\n--- " << request_name << " ---" << std::endl;

    // Check for CPR errors first
    if (r.error.code != cpr::ErrorCode::OK) {
        std::cout << "❌ CPR Error: [" << static_cast<int>(r.error.code) << "] " << r.error.message << std::endl;
        return;
    }

    std::cout << "Status Code: " << r.status_code;
    if (r.status_code >= 200 && r.status_code < 300) {
        std::cout << " ✅";
    }
    else if (r.status_code >= 400) {
        std::cout << " ❌";
    }
    else {
        std::cout << " ⚠️";
    }
    std::cout << std::endl;

    std::cout << "URL: " << r.url << std::endl;
    std::cout << "Elapsed Time: " << r.elapsed << "s" << std::endl;

    // Show important headers
    std::cout << "Key Headers:" << std::endl;
    for (const auto& pair : r.header) {
        if (pair.first == "content-type" || pair.first == "server" ||
            pair.first == "content-length" || pair.first.find("x-") == 0) {
            std::cout << "  " << pair.first << ": " << pair.second << std::endl;
        }
    }

    // Show response body (truncated for readability)
    if (!r.text.empty()) {
        std::cout << "Response Body:" << std::endl;
        if (r.text.length() > 300) {
            std::cout << r.text.substr(0, 300) << "..." << std::endl;
        }
        else {
            std::cout << r.text << std::endl;
        }
    }
}

void printSectionHeader(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << std::string(60, '=') << std::endl;
}

int main() {
    std::cout << "🚀 CPR Library Test Suite" << std::endl;
    std::cout << "Testing HTTP client functionality with CPR (libcurl wrapper)" << std::endl;
    std::cout << "Using httpbin.org for testing endpoints\n" << std::endl;

    // --- 1. Basic GET Request ---
    printSectionHeader("1. Basic GET Request (HTTPS)");
    auto r_get = cpr::Get(
        cpr::Url{ "https://httpbin.org/get" },
        cpr::Parameters{ {"hello", "world"}, {"test", "cpr"} }
    );
    printResponseDetails(r_get, "Basic GET with parameters");

    // --- 2. POST Request with Form Data ---
    printSectionHeader("2. POST Request (Form Data)");
    auto r_post_form = cpr::Post(
        cpr::Url{ "https://httpbin.org/post" },
        cpr::Payload{ {"username", "testuser"}, {"message", "Hello from CPR!"} }
    );
    printResponseDetails(r_post_form, "POST with form data");

    // --- 3. POST Request with JSON ---
    printSectionHeader("3. POST Request (JSON Data)");
    std::string json_payload = R"({
        "name": "CPR Test",
        "version": "1.0",
        "features": ["http", "https", "json"],
        "active": true
    })";
    auto r_post_json = cpr::Post(
        cpr::Url{ "https://httpbin.org/post" },
        cpr::Body{ json_payload },
        cpr::Header{ {"Content-Type", "application/json"} }
    );
    printResponseDetails(r_post_json, "POST with JSON data");

    // --- 4. Custom Headers ---
    printSectionHeader("4. Custom Headers");
    auto r_headers = cpr::Get(
        cpr::Url{ "https://httpbin.org/headers" },
        cpr::Header{
            {"User-Agent", "CPR-Test-Client/1.0"},
            {"X-API-Key", "test-key-123"},
            {"Accept", "application/json"}
        }
    );
    printResponseDetails(r_headers, "GET with custom headers");

    // --- 5. Timeout Handling ---
    printSectionHeader("5. Timeout Testing");
    std::cout << "Testing short timeout (should timeout)..." << std::endl;
    auto r_timeout = cpr::Get(
        cpr::Url{ "https://httpbin.org/delay/3" },  // 3 second delay
        cpr::Timeout{ 1000 }  // 1 second timeout
    );
    printResponseDetails(r_timeout, "GET with short timeout");

    std::cout << "\nTesting adequate timeout..." << std::endl;
    auto r_timeout_ok = cpr::Get(
        cpr::Url{ "https://httpbin.org/delay/1" },  // 1 second delay
        cpr::Timeout{ 3000 }  // 3 second timeout
    );
    printResponseDetails(r_timeout_ok, "GET with adequate timeout");

    // --- 6. Basic Authentication ---
    printSectionHeader("6. Basic Authentication");
    auto r_auth_success = cpr::Get(
        cpr::Url{ "https://httpbin.org/basic-auth/user/pass" },
        cpr::Authentication{ "user", "pass", cpr::AuthMode::BASIC }
    );
    printResponseDetails(r_auth_success, "Basic auth (correct credentials)");

    auto r_auth_fail = cpr::Get(
        cpr::Url{ "https://httpbin.org/basic-auth/user/pass" },
        cpr::Authentication{ "wrong", "credentials", cpr::AuthMode::BASIC }
    );
    printResponseDetails(r_auth_fail, "Basic auth (wrong credentials)");

    // --- 7. Cookie Handling ---
    printSectionHeader("7. Cookie Handling");
    // Set cookies
    auto r_cookie_set = cpr::Get(
        cpr::Url{ "https://httpbin.org/cookies/set" },
        cpr::Parameters{ {"sessionid", "abc123"}, {"theme", "dark"} }
    );
    printResponseDetails(r_cookie_set, "Setting cookies");

    // Send cookies manually
    auto r_cookie_send = cpr::Get(
        cpr::Url{ "https://httpbin.org/cookies" },
        cpr::Cookies{ {"client_id", "test123"}, {"preferences", "json"} }
    );
    printResponseDetails(r_cookie_send, "Sending cookies");

    // --- 8. Redirect Handling ---
    printSectionHeader("8. Redirect Handling");
    auto r_redirect = cpr::Get(cpr::Url{ "https://httpbin.org/redirect/2" });
    printResponseDetails(r_redirect, "Following redirects (default)");

    auto r_redirect_limited = cpr::Get(
        cpr::Url{ "https://httpbin.org/redirect/3" },
        cpr::Redirect{ 1L }  // Allow only 1 redirect
    );
    printResponseDetails(r_redirect_limited, "Limited redirects (max 1)");

    // --- 9. Asynchronous Request ---
    printSectionHeader("9. Asynchronous Request");
    std::cout << "Starting async request..." << std::endl;
    auto future_response = cpr::GetAsync(cpr::Url{ "https://httpbin.org/delay/1" });

    std::cout << "Doing other work while request is in progress..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "Waiting for async response..." << std::endl;
    auto r_async = future_response.get();
    printResponseDetails(r_async, "Async GET request");

    // --- 10. Session Usage ---
    printSectionHeader("10. Session-based Requests");
    cpr::Session session;
    session.SetUrl(cpr::Url{ "https://httpbin.org/get" });
    session.SetHeader(cpr::Header{ {"X-Session-ID", "session123"} });
    session.SetTimeout(cpr::Timeout{ 5000 });

    auto r_session1 = session.Get();
    printResponseDetails(r_session1, "Session GET #1");

    // Modify session for another request
    session.SetUrl(cpr::Url{ "https://httpbin.org/post" });
    session.SetPayload(cpr::Payload{ {"session_data", "persistent"} });
    auto r_session2 = session.Post();
    printResponseDetails(r_session2, "Session POST #2");

    // --- 11. Different HTTP Methods ---
    printSectionHeader("11. Various HTTP Methods");

    auto r_put = cpr::Put(
        cpr::Url{ "https://httpbin.org/put" },
        cpr::Body{ "Updated content" }
    );
    printResponseDetails(r_put, "PUT request");

    auto r_delete = cpr::Delete(cpr::Url{ "https://httpbin.org/delete" });
    printResponseDetails(r_delete, "DELETE request");

    auto r_patch = cpr::Patch(
        cpr::Url{ "https://httpbin.org/patch" },
        cpr::Body{ R"({"operation": "update", "field": "name"})" },
        cpr::Header{ {"Content-Type", "application/json"} }
    );
    printResponseDetails(r_patch, "PATCH request");

    // --- 12. Error Handling ---
    printSectionHeader("12. Error Handling");
    auto r_invalid = cpr::Get(cpr::Url{ "https://invalid-domain-that-should-not-exist-12345.com" });
    printResponseDetails(r_invalid, "Invalid domain (DNS error expected)");

    auto r_404 = cpr::Get(cpr::Url{ "https://httpbin.org/status/404" });
    printResponseDetails(r_404, "404 Not Found");

    // --- Summary ---
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "🎉 CPR Test Suite Complete!" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "\nIf you see mostly ✅ symbols and status codes 200-299," << std::endl;
    std::cout << "then CPR is working correctly with your setup!" << std::endl;
    std::cout << "\nFeatures tested:" << std::endl;
    std::cout << "- GET/POST/PUT/DELETE/PATCH requests" << std::endl;
    std::cout << "- HTTPS support" << std::endl;
    std::cout << "- JSON and form data" << std::endl;
    std::cout << "- Custom headers" << std::endl;
    std::cout << "- Authentication" << std::endl;
    std::cout << "- Cookies" << std::endl;
    std::cout << "- Timeouts" << std::endl;
    std::cout << "- Redirects" << std::endl;
    std::cout << "- Async requests" << std::endl;
    std::cout << "- Sessions" << std::endl;
    std::cout << "- Error handling" << std::endl;

    std::cout << "\nPress Enter to exit...";
    std::cin.get();

    return 0;
}