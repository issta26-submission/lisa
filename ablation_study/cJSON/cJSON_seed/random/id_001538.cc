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
//<ID> 1538
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
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved_payload = (cJSON *)0;
    cJSON *retrieved_title = (cJSON *)0;
    const char *title_str = (const char *)0;
    cJSON_bool added_str = 0;
    cJSON_bool added_num = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("hello");
    num_item = cJSON_CreateNumber(123.0);

    // step 3: Configure
    added_str = cJSON_AddItemToArray(arr, str_item);
    added_num = cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "payload", arr);
    cJSON_AddStringToObject(root, "title", "greeting");

    // step 4: Operate
    retrieved_payload = cJSON_GetObjectItem(root, "payload");
    retrieved_title = cJSON_GetObjectItem(root, "title");
    title_str = cJSON_GetStringValue(retrieved_title);

    // step 5: Validate
    validation_score = (int)added_str + (int)added_num + (int)(retrieved_payload != (cJSON *)0) + (int)(retrieved_title != (cJSON *)0) + (int)(title_str != (const char *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)title_str;
    (void)retrieved_payload;
    (void)retrieved_title;
    // API sequence test completed successfully
    return 66;
}