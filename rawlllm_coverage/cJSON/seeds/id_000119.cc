#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 119
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Create a root JSON object
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 1;
    }

    // Add a string member: "name": "Alice"
    cJSON *name = cJSON_CreateString("Alice");
    if (!name) {
        cJSON_Delete(root);
        return 2;
    }
    cJSON_AddItemToObject(root, "name", name);

    // Add a number member: "age": 30
    cJSON *age = cJSON_CreateNumber(30);
    if (!age) {
        cJSON_Delete(root);
        return 3;
    }
    cJSON_AddItemToObject(root, "age", age);

    // Add a boolean member: "active": true
    // Many cJSON distributions provide cJSON_CreateTrue(); use that to create a bool node.
    cJSON *active = cJSON_CreateTrue();
    if (!active) {
        cJSON_Delete(root);
        return 4;
    }
    cJSON_AddItemToObject(root, "active", active);

    // Create an array "hobbies": ["reading", "coding"]
    cJSON *hobbies = cJSON_CreateArray();
    if (!hobbies) {
        cJSON_Delete(root);
        return 5;
    }
    cJSON_AddItemToObject(root, "hobbies", hobbies);
    // add items to array (each CreateString returns a new cJSON* which becomes owned by the array)
    cJSON_AddItemToArray(hobbies, cJSON_CreateString("reading"));
    cJSON_AddItemToArray(hobbies, cJSON_CreateString("coding"));

    // Retrieve and validate some members from the root object
    cJSON *got_name = cJSON_GetObjectItemCaseSensitive(root, "name");
    if (!cJSON_IsString(got_name) || (got_name->valuestring == NULL)) {
        cJSON_Delete(root);
        return 6;
    }
    cJSON *got_age = cJSON_GetObjectItemCaseSensitive(root, "age");
    if (!cJSON_IsNumber(got_age)) {
        cJSON_Delete(root);
        return 7;
    }

    // Duplicate the root object (deep copy)
    cJSON *copy = cJSON_Duplicate(root, /*recurse=*/1);
    if (!copy) {
        cJSON_Delete(root);
        return 8;
    }

    // Detach the "hobbies" array from the duplicate to operate on it independently
    cJSON *detached_hobbies = cJSON_DetachItemFromObject(copy, "hobbies");
    if (!detached_hobbies) {
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return 9;
    }
    // Add another hobby to the detached array to show it's independent from the original
    cJSON_AddItemToArray(detached_hobbies, cJSON_CreateString("music"));

    // Parse a JSON string (an array of objects) and validate its contents
    const char *json_text = "[{\"id\":1,\"val\":\"one\"},{\"id\":2,\"val\":\"two\"}]";
    cJSON *parsed = cJSON_Parse(json_text);
    if (!parsed) {
        cJSON_Delete(detached_hobbies);
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return 10;
    }
    if (!cJSON_IsArray(parsed)) {
        cJSON_Delete(parsed);
        cJSON_Delete(detached_hobbies);
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return 11;
    }
    cJSON *first = cJSON_GetArrayItem(parsed, 0);
    if (!first || !cJSON_IsObject(first)) {
        cJSON_Delete(parsed);
        cJSON_Delete(detached_hobbies);
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return 12;
    }
    cJSON *first_id = cJSON_GetObjectItemCaseSensitive(first, "id");
    if (!cJSON_IsNumber(first_id) || first_id->valueint != 1) {
        cJSON_Delete(parsed);
        cJSON_Delete(detached_hobbies);
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return 13;
    }

    // Clean up all allocated cJSON structures
    cJSON_Delete(parsed);
    cJSON_Delete(detached_hobbies);
    cJSON_Delete(copy);
    cJSON_Delete(root);

    // If we reached here, the sequence of API calls succeeded
    return 66;
}