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
//<ID> 38
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // This function demonstrates a sequence of common cJSON API calls:
    // create objects/arrays/values, serialize, parse, inspect/replace items,
    // duplicate, and cleanup. It returns 66 on success.
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;

    // Add simple items
    cJSON_AddStringToObject(root, "name", "Alice");
    cJSON_AddNumberToObject(root, "age", 30);

    // Create and add an array
    cJSON *languages = cJSON_CreateArray();
    if (!languages) { cJSON_Delete(root); return 0; }
    cJSON_AddItemToArray(languages, cJSON_CreateString("C++"));
    cJSON_AddItemToArray(languages, cJSON_CreateString("Python"));
    cJSON_AddItemToObject(root, "languages", languages);

    // Create and add a nested object
    cJSON *address = cJSON_CreateObject();
    if (!address) { cJSON_Delete(root); return 0; }
    cJSON_AddStringToObject(address, "city", "Wonderland");
    cJSON_AddNumberToObject(address, "zip", 12345);
    cJSON_AddItemToObject(root, "address", address);

    // Serialize to string
    char *printed = cJSON_Print(root);
    if (!printed) { cJSON_Delete(root); return 0; }

    // Parse back the JSON text
    cJSON *parsed = cJSON_Parse(printed);
    if (!parsed) {
        cJSON_free(printed);
        cJSON_Delete(root);
        return 0;
    }

    // Inspect some items from parsed JSON
    cJSON *pname = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    if (!cJSON_IsString(pname) || (pname->valuestring == NULL)) {
        cJSON_free(printed);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 0;
    }

    cJSON *page = cJSON_GetObjectItemCaseSensitive(parsed, "age");
    if (!cJSON_IsNumber(page)) {
        cJSON_free(printed);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 0;
    }

    // Replace the age in the parsed object
    if (!cJSON_ReplaceItemInObject(parsed, "age", cJSON_CreateNumber(31))) {
        cJSON_free(printed);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 0;
    }

    // Add another language to the languages array in parsed JSON
    cJSON *plang = cJSON_GetObjectItemCaseSensitive(parsed, "languages");
    if (!cJSON_IsArray(plang)) {
        cJSON_free(printed);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 0;
    }
    cJSON_AddItemToArray(plang, cJSON_CreateString("Rust"));

    // Duplicate the parsed object (deep copy)
    cJSON *dup = cJSON_Duplicate(parsed, /*recurse=*/1);
    if (!dup) {
        cJSON_free(printed);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 0;
    }

    // Sanity-check the duplicate has the replaced age
    cJSON *dup_age = cJSON_GetObjectItemCaseSensitive(dup, "age");
    if (!cJSON_IsNumber(dup_age) || dup_age->valuedouble != 31) {
        cJSON_free(printed);
        cJSON_Delete(dup);
        cJSON_Delete(parsed);
        cJSON_Delete(root);
        return 0;
    }

    // Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // Success
    return 66;
}