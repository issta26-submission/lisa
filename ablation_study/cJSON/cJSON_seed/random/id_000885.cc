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
//<ID> 885
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *text = "hello world";
    const double answer = 42.0;
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved_str_obj = (cJSON *)0;
    cJSON *retrieved_num_obj = (cJSON *)0;
    const char *retrieved_text = (const char *)0;
    double retrieved_number = 0.0;
    cJSON_bool add_ok = (cJSON_bool)0;
    cJSON_bool is_true_flag = (cJSON_bool)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize root object and create a string item
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString(text);

    // step 3: Configure object by adding created items (string, true, number)
    add_ok = cJSON_AddItemToObject(root, "greeting", str_item);
    true_item = cJSON_AddTrueToObject(root, "is_valid");
    num_item = cJSON_AddNumberToObject(root, "answer", answer);

    // step 4: Operate by retrieving items and extracting their values
    retrieved_str_obj = cJSON_GetObjectItem(root, "greeting");
    retrieved_text = cJSON_GetStringValue(retrieved_str_obj);
    retrieved_num_obj = cJSON_GetObjectItem(root, "answer");
    retrieved_number = cJSON_GetNumberValue(retrieved_num_obj);
    is_true_flag = cJSON_IsTrue(cJSON_GetObjectItem(root, "is_valid"));

    // step 5: Validate by printing and aggregating simple checks
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok + (int)(true_item != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(retrieved_text != (const char *)0) + (int)(retrieved_number == answer) + (int)is_true_flag + (int)(printed != (char *)0);
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}