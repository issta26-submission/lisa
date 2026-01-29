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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Create a root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return 0;

    // Add simple members
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNumberToObject(root, "age", 30);

    // Create an array and add it to root
    cJSON *arr = cJSON_CreateArray();
    if (!arr) { cJSON_Delete(root); return 0; }
    cJSON_AddItemToArray(arr, cJSON_CreateString("one"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));
    cJSON_AddItemToObject(root, "items", arr);

    // Create a nested object and add it to root
    cJSON *address = cJSON_CreateObject();
    if (!address) { cJSON_Delete(root); return 0; }
    cJSON_AddStringToObject(address, "city", "Metropolis");
    cJSON_AddNumberToObject(address, "zip", 12345);
    cJSON_AddItemToObject(root, "address", address);

    // Duplicate the root (deep duplication)
    cJSON *copy = cJSON_Duplicate(root, 1);
    if (!copy) { cJSON_Delete(root); return 0; }

    // Replace the "name" field in the copy
    cJSON_ReplaceItemInObject(copy, "name", cJSON_CreateString("copy_of_example"));

    // Detach "age" from the copy and add it back into the original under a different key
    cJSON *detached_age = cJSON_DetachItemFromObject(copy, "age");
    if (!detached_age) {
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return 0;
    }
    cJSON_AddItemToObject(root, "age_detached", detached_age);

    // Parse a JSON string and add a duplicate of the parsed object into root
    const char *json_text = "{\"parsed\":true, \"value\":123}";
    cJSON *parsed = cJSON_Parse(json_text);
    if (!parsed) {
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return 0;
    }
    cJSON_AddItemToObject(root, "from_text", cJSON_Duplicate(parsed, 1));

    // Validate that the inserted parsed content is present and correct
    cJSON *from_text = cJSON_GetObjectItemCaseSensitive(root, "from_text");
    if (!from_text) {
        cJSON_Delete(parsed);
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return 0;
    }
    cJSON *parsed_flag = cJSON_GetObjectItemCaseSensitive(from_text, "parsed");
    if (!parsed_flag || !cJSON_IsTrue(parsed_flag)) {
        cJSON_Delete(parsed);
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return 0;
    }
    cJSON *value_item = cJSON_GetObjectItemCaseSensitive(from_text, "value");
    if (!value_item || !cJSON_IsNumber(value_item) || (int)(value_item->valuedouble) != 123) {
        cJSON_Delete(parsed);
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return 0;
    }

    // Verify the detached age exists in root and has the expected value
    cJSON *age_det = cJSON_GetObjectItemCaseSensitive(root, "age_detached");
    if (!age_det || !cJSON_IsNumber(age_det) || (int)(age_det->valuedouble) != 30) {
        cJSON_Delete(parsed);
        cJSON_Delete(copy);
        cJSON_Delete(root);
        return 0;
    }

    // Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(copy);
    cJSON_Delete(root);

    // On success, return the required value
    return 66;
}