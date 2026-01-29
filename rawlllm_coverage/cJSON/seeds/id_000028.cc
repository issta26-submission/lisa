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
//<ID> 28
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Create a root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return -1;

    // Add basic fields: string and number
    cJSON *name = cJSON_CreateString("example");
    if (!name) { cJSON_Delete(root); return -1; }
    cJSON_AddItemToObject(root, "name", name);

    cJSON *age = cJSON_CreateNumber(30);
    if (!age) { cJSON_Delete(root); return -1; }
    cJSON_AddItemToObject(root, "age", age);

    // Add a nested object "address"
    cJSON *address = cJSON_CreateObject();
    if (!address) { cJSON_Delete(root); return -1; }
    cJSON_AddItemToObject(root, "address", address);

    cJSON *city = cJSON_CreateString("Springfield");
    if (!city) { cJSON_Delete(root); return -1; }
    cJSON_AddItemToObject(address, "city", city);

    cJSON *zip = cJSON_CreateNumber(12345);
    if (!zip) { cJSON_Delete(root); return -1; }
    cJSON_AddItemToObject(address, "zip", zip);

    // Add an array "scores"
    cJSON *scores = cJSON_CreateArray();
    if (!scores) { cJSON_Delete(root); return -1; }
    cJSON_AddItemToObject(root, "scores", scores);

    // Populate the array
    for (int i = 0; i < 3; ++i) {
        cJSON *score = cJSON_CreateNumber((i + 1) * 10);
        if (!score) { cJSON_Delete(root); return -1; }
        cJSON_AddItemToArray(scores, score);
    }

    // Add a boolean and a null value
    cJSON *active = cJSON_CreateTrue();
    if (!active) { cJSON_Delete(root); return -1; }
    cJSON_AddItemToObject(root, "active", active);

    cJSON *nullable = cJSON_CreateNull();
    if (!nullable) { cJSON_Delete(root); return -1; }
    cJSON_AddItemToObject(root, "optional", nullable);

    // Convert to string (compact) and parse back
    char *json_str = cJSON_PrintUnformatted(root);
    if (!json_str) { cJSON_Delete(root); return -1; }

    cJSON *parsed = cJSON_Parse(json_str);
    // free the printed string via cJSON_free (defined by cJSON)
    cJSON_free(json_str);
    if (!parsed) { cJSON_Delete(root); return -1; }

    // Validate parsed content: check name
    cJSON *p_name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(p_name) || (p_name->valuestring == NULL) || strcmp(p_name->valuestring, "example") != 0) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -1;
    }

    // Check age
    cJSON *p_age = cJSON_GetObjectItemCaseSensitive(parsed, "age");
    if (!cJSON_IsNumber(p_age) || p_age->valuedouble != 30) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -1;
    }

    // Check address->city
    cJSON *p_address = cJSON_GetObjectItemCaseSensitive(parsed, "address");
    if (!cJSON_IsObject(p_address)) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -1;
    }
    cJSON *p_city = cJSON_GetObjectItemCaseSensitive(p_address, "city");
    if (!cJSON_IsString(p_city) || (p_city->valuestring == NULL) || strcmp(p_city->valuestring, "Springfield") != 0) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -1;
    }

    // Check scores array and its contents
    cJSON *p_scores = cJSON_GetObjectItemCaseSensitive(parsed, "scores");
    if (!cJSON_IsArray(p_scores) || cJSON_GetArraySize(p_scores) != 3) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -1;
    }
    for (int i = 0; i < 3; ++i) {
        cJSON *item = cJSON_GetArrayItem(p_scores, i);
        if (!cJSON_IsNumber(item) || item->valuedouble != (i + 1) * 10) {
            cJSON_Delete(parsed);
            cJSON_Delete(root);
            return -1;
        }
    }

    // Modify parsed: add a new field and remove "age"
    cJSON *new_field = cJSON_CreateNumber(42);
    if (!new_field) { cJSON_Delete(parsed); cJSON_Delete(root); return -1; }
    cJSON_AddItemToObject(parsed, "new_field", new_field);

    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "age");
    // Ensure "age" is gone
    if (cJSON_GetObjectItemCaseSensitive(parsed, "age") != NULL) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -1;
    }

    // Create a string from modified parsed object and ensure it exists
    char *modified_str = cJSON_PrintUnformatted(parsed);
    if (!modified_str) {
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return -1;
    }
    // free the modified string
    cJSON_free(modified_str);

    // Clean up all cJSON objects
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // If we reached here, the sequence succeeded
    return 66;
}