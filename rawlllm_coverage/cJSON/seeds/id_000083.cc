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
//<ID> 83
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Build a JSON object:
    // {
    //   "name": "Alice",
    //   "age": 30,
    //   "married": true,
    //   "children": ["Bob", "Carol"],
    //   "address": { "street": "123 Main St", "city": "Metropolis", "zip": 12345 }
    // }
    cJSON *root = cJSON_CreateObject();
    if (!root) return -1;

    cJSON_AddStringToObject(root, "name", "Alice");
    cJSON_AddNumberToObject(root, "age", 30);
    // Use cJSON_CreateTrue() which returns a cJSON boolean true item
    cJSON_AddItemToObject(root, "married", cJSON_CreateTrue());

    // children array
    cJSON *children = cJSON_CreateArray();
    if (!children) { cJSON_Delete(root); return -2; }
    cJSON_AddItemToArray(children, cJSON_CreateString("Bob"));
    cJSON_AddItemToArray(children, cJSON_CreateString("Carol"));
    cJSON_AddItemToObject(root, "children", children);

    // nested address object
    cJSON *address = cJSON_CreateObject();
    if (!address) { cJSON_Delete(root); return -3; }
    cJSON_AddStringToObject(address, "street", "123 Main St");
    cJSON_AddStringToObject(address, "city", "Metropolis");
    cJSON_AddNumberToObject(address, "zip", 12345);
    cJSON_AddItemToObject(root, "address", address);

    // Convert to string (compact)
    char *json_str = cJSON_PrintUnformatted(root);
    if (!json_str) { cJSON_Delete(root); return -4; }

    // Parse the produced string back into a cJSON structure
    cJSON *parsed = cJSON_Parse(json_str);
    // free the printed string (cJSON_Print/cJSON_PrintUnformatted allocate with malloc)
    free(json_str);

    if (!parsed) { cJSON_Delete(root); return -5; }

    // Validate some fields in the parsed tree

    // name
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(name) || (name->valuestring == NULL) || (strcmp(name->valuestring, "Alice") != 0)) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -6;
    }

    // age
    cJSON *age = cJSON_GetObjectItemCaseSensitive(parsed, "age");
    if (!cJSON_IsNumber(age) || (age->valuedouble != 30)) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -7;
    }

    // married
    cJSON *married = cJSON_GetObjectItemCaseSensitive(parsed, "married");
    if (!cJSON_IsTrue(married)) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -8;
    }

    // children array and its items
    cJSON *children_p = cJSON_GetObjectItemCaseSensitive(parsed, "children");
    if (!cJSON_IsArray(children_p) || (cJSON_GetArraySize(children_p) != 2)) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -9;
    }
    cJSON *child0 = cJSON_GetArrayItem(children_p, 0);
    cJSON *child1 = cJSON_GetArrayItem(children_p, 1);
    if (!cJSON_IsString(child0) || !cJSON_IsString(child1) ||
        strcmp(child0->valuestring, "Bob") != 0 || strcmp(child1->valuestring, "Carol") != 0) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -10;
    }

    // nested address checks
    cJSON *address_p = cJSON_GetObjectItemCaseSensitive(parsed, "address");
    if (!cJSON_IsObject(address_p)) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -11;
    }
    cJSON *street = cJSON_GetObjectItemCaseSensitive(address_p, "street");
    cJSON *city = cJSON_GetObjectItemCaseSensitive(address_p, "city");
    cJSON *zip = cJSON_GetObjectItemCaseSensitive(address_p, "zip");
    if (!cJSON_IsString(street) || !cJSON_IsString(city) || !cJSON_IsNumber(zip) ||
        strcmp(street->valuestring, "123 Main St") != 0 || strcmp(city->valuestring, "Metropolis") != 0 ||
        (zip->valuedouble != 12345)) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -12;
    }

    // Demonstrate modifying the parsed tree: add a new field
    cJSON_AddStringToObject(parsed, "new_field", "new_value");
    cJSON *new_field = cJSON_GetObjectItemCaseSensitive(parsed, "new_field");
    if (!cJSON_IsString(new_field) || strcmp(new_field->valuestring, "new_value") != 0) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -13;
    }

    // Clean up both trees
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // If we reached here, the sequence of cJSON API calls succeeded and validated
    return 66;
}