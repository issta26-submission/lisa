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
//<ID> 49
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function demonstrates a safe, logically-correct sequence of cJSON API calls.
    // It builds a JSON object, serializes it, parses the serialized string back,
    // validates fields, duplicates and mutates the duplicate, then cleans up.
    //
    // Note: this function assumes the project that compiles it includes the
    // appropriate cJSON header (e.g. #include "cJSON.h") and standard headers
    // (for free() and strcmp). Error branches free allocated resources before returning.

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    // Add some primitive fields
    if (!cJSON_AddStringToObject(root, "name", "example")) {
        cJSON_Delete(root);
        return 0;
    }
    if (!cJSON_AddNumberToObject(root, "age", 42)) {
        cJSON_Delete(root);
        return 0;
    }

    // Create an array and populate it
    cJSON *items = cJSON_CreateArray();
    if (!items) {
        cJSON_Delete(root);
        return 0;
    }
    // Note: cJSON_CreateString / cJSON_CreateNumber return new cJSON nodes that are
    // owned by the array after cJSON_AddItemToArray.
    cJSON_AddItemToArray(items, cJSON_CreateString("one"));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(2));
    cJSON_AddItemToObject(root, "items", items); // items now owned by root

    // Create a nested object
    cJSON *nested = cJSON_CreateObject();
    if (!nested) {
        cJSON_Delete(root);
        return 0;
    }
    cJSON_AddStringToObject(nested, "inner", "value");
    cJSON_AddItemToObject(root, "nested", nested); // nested now owned by root

    // Serialize to string
    char *json_str = cJSON_Print(root);
    if (!json_str) {
        cJSON_Delete(root);
        return 0;
    }

    // Parse the JSON string back into a new cJSON structure
    cJSON *parsed = cJSON_Parse(json_str);
    if (!parsed) {
        // Clean up everything that was created
        free(json_str);
        cJSON_Delete(root);
        return 0;
    }

    // Validate some fields
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(name) || (name->valuestring == NULL) || strcmp(name->valuestring, "example") != 0) {
        cJSON_Delete(parsed);
        free(json_str);
        cJSON_Delete(root);
        return 0;
    }

    cJSON *age = cJSON_GetObjectItemCaseSensitive(parsed, "age");
    if (!cJSON_IsNumber(age) || (age->valuedouble != 42)) {
        cJSON_Delete(parsed);
        free(json_str);
        cJSON_Delete(root);
        return 0;
    }

    cJSON *parsed_items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    if (!cJSON_IsArray(parsed_items) || cJSON_GetArraySize(parsed_items) < 2) {
        cJSON_Delete(parsed);
        free(json_str);
        cJSON_Delete(root);
        return 0;
    }
    cJSON *first_item = cJSON_GetArrayItem(parsed_items, 0);
    if (!cJSON_IsString(first_item) || (first_item->valuestring == NULL) || strcmp(first_item->valuestring, "one") != 0) {
        cJSON_Delete(parsed);
        free(json_str);
        cJSON_Delete(root);
        return 0;
    }

    // Duplicate the parsed object (deep duplicate)
    cJSON *dup = cJSON_Duplicate(parsed, 1); // 1 == recurse
    if (!dup) {
        cJSON_Delete(parsed);
        free(json_str);
        cJSON_Delete(root);
        return 0;
    }

    // Mutate the duplicate: remove the "age" field and add a new field
    cJSON_DeleteItemFromObject(dup, "age");
    if (!cJSON_AddNumberToObject(dup, "new_field", 100)) {
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        free(json_str);
        cJSON_Delete(root);
        return 0;
    }

    // Optionally serialize the duplicate (to ensure the mutated structure is valid)
    char *dup_str = cJSON_Print(dup);
    if (dup_str) {
        free(dup_str);
    } // if duplication serialization failed, we still proceed to cleanup

    // Clean up all allocated cJSON structures and serialized strings
    cJSON_Delete(dup);
    cJSON_Delete(parsed);
    free(json_str);
    cJSON_Delete(root);

    // If we reached here, the API sequence succeeded
    return 66;
}