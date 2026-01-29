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
//<ID> 448
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *letters = cJSON_CreateArray();
    cJSON *meta = cJSON_CreateObject();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *s3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(letters, s1);
    cJSON_AddItemToArray(letters, s2);
    cJSON_AddItemToArray(letters, s3);
    cJSON_AddItemToObject(root, "letters", letters);
    cJSON_AddStringToObject(root, "title", "DemoDocument");
    cJSON_AddStringToObject(root, "author", "tester@example.com");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddNumberToObject(meta, "initial_count", 3.0);
    cJSON_AddStringToObject(meta, "category", "examples");

    // step 3: Operate
    cJSON *letters_from_root = cJSON_GetObjectItem(root, "letters");
    cJSON *first_item = cJSON_GetArrayItem(letters_from_root, 0);
    cJSON *second_item = cJSON_GetArrayItem(letters_from_root, 1);
    cJSON_bool first_is_str = cJSON_IsString(first_item);
    cJSON_bool second_is_str = cJSON_IsString(second_item);
    cJSON_AddNumberToObject(root, "first_is_string", (double)first_is_str);
    cJSON_AddNumberToObject(root, "second_is_string", (double)second_is_str);
    const char *first_val = cJSON_GetStringValue(first_item);
    cJSON_AddStringToObject(meta, "first_letter", first_val);
    cJSON_DeleteItemFromArray(letters_from_root, 1);
    int remaining = cJSON_GetArraySize(letters_from_root);
    cJSON_AddNumberToObject(root, "letters_size", (double)remaining);

    // step 4: Validate and Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}