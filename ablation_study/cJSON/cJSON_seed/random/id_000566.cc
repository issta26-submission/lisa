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
//<ID> 566
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
    cJSON *entry1 = (cJSON *)0;
    cJSON *entry2 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_elem = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    char *json_text = (char *)0;
    double extracted_value = 0.0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    entry1 = cJSON_CreateObject();
    entry2 = cJSON_CreateObject();

    // step 3: Configure
    num1 = cJSON_AddNumberToObject(entry1, "value", 10.5);
    num2 = cJSON_AddNumberToObject(entry2, "value", 20.25);
    add_ok1 = cJSON_AddItemToArray(arr, entry1);
    add_ok2 = cJSON_AddItemToArray(arr, entry2);
    add_ok3 = cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    json_text = cJSON_PrintUnformatted(root);
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    retrieved_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    retrieved_num = cJSON_GetObjectItem(retrieved_elem, "value");
    extracted_value = cJSON_GetNumberValue(retrieved_num);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)num1;
    (void)num2;
    (void)json_text;
    (void)retrieved_arr;
    (void)retrieved_elem;
    (void)retrieved_num;
    (void)extracted_value;

    // step 6: Cleanup
    if (json_text) {
        cJSON_free(json_text);
    }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}