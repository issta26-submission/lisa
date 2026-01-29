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
//<ID> 1516
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *person = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("Bob");
    cJSON *age = cJSON_CreateNumber(28.0);
    cJSON *scores = cJSON_CreateArray();
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(88.5));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(91.0));
    cJSON_AddItemToObject(person, "name", name);
    cJSON_AddItemToObject(person, "age", age);
    cJSON_AddItemToObject(person, "scores", scores);
    cJSON_AddItemToObject(root, "person", person);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe_null);
    const char json_text[] = "{\"alpha\":42,\"beta\":[7,8]}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_text, (size_t)(sizeof(json_text) - 1), &parse_end, 1);

    // step 2: Configure
    const int nums[3] = {1, 2, 3};
    cJSON *int_array = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON *dup_parsed = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "dup_parsed", dup_parsed);

    // step 3: Operate & Validate
    cJSON_bool has_person = cJSON_HasObjectItem(root, "person");
    cJSON *person_ref = cJSON_GetObjectItemCaseSensitive(root, "person");
    cJSON *maybe_ref = cJSON_GetObjectItem(root, "maybe");
    cJSON_bool maybe_is_null = cJSON_IsNull(maybe_ref);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "dup_parsed");
    cJSON *duplicate_root = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, duplicate_root, 1);
    (void)has_person;
    (void)person_ref;
    (void)maybe_is_null;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(duplicate_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}