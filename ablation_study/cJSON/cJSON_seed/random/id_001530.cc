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
//<ID> 1530
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *retrieved_list = (cJSON *)0;
    cJSON *retrieved_title_obj = (cJSON *)0;
    char *printed = (char *)0;
    const char *retrieved_title = (const char *)0;
    cJSON_bool added_str1 = 0;
    cJSON_bool added_str2 = 0;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_list_to_root = 0;
    cJSON_bool added_title_to_root = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("alpha");
    str2 = cJSON_CreateString("beta");
    num1 = cJSON_CreateNumber(100.0);

    // step 3: Configure
    added_str1 = cJSON_AddItemToArray(arr, str1);
    added_str2 = cJSON_AddItemToArray(arr, str2);
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_list_to_root = cJSON_AddItemToObject(root, "list", arr);
    added_title_to_root = cJSON_AddItemToObject(root, "title", cJSON_CreateString("example"));

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_title_obj = cJSON_GetObjectItem(root, "title");
    retrieved_title = cJSON_GetStringValue(retrieved_title_obj);
    retrieved_list = cJSON_GetObjectItem(root, "list");

    // step 5: Validate
    validation_score = (int)added_str1 + (int)added_str2 + (int)added_num1 + (int)added_list_to_root + (int)added_title_to_root + (int)(printed != (char *)0) + (int)(retrieved_title != (const char *)0) + (int)(retrieved_list != (cJSON *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_title;
    (void)retrieved_list;
    (void)retrieved_title_obj;
    // API sequence test completed successfully
    return 66;
}