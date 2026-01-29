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
//<ID> 1515
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
    cJSON_AddStringToObject(person, "name", "Bob");
    cJSON_AddNumberToObject(person, "age", 28.0);
    cJSON *middle_null = cJSON_CreateNull();
    cJSON_AddItemToObject(person, "middle_name", middle_null);

    // step 2: Configure
    cJSON *scores = cJSON_CreateArray();
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(75.0));
    cJSON_AddItemToArray(scores, cJSON_CreateNumber(88.5));
    cJSON_AddItemToObject(person, "scores", scores);
    cJSON_AddItemToObject(root, "person", person);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    cJSON_bool has_middle_before = cJSON_HasObjectItem(person, "middle_name");
    cJSON *middle_item = cJSON_GetObjectItem(person, "middle_name");
    cJSON_bool middle_is_null = cJSON_IsNull(middle_item);
    cJSON_DeleteItemFromObjectCaseSensitive(person, "middle_name");
    cJSON_bool has_middle_after = cJSON_HasObjectItem(person, "middle_name");
    cJSON *middle_present_flag = cJSON_CreateBool(has_middle_after);
    cJSON_AddItemToObject(root, "middle_present", middle_present_flag);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(duplicate);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}