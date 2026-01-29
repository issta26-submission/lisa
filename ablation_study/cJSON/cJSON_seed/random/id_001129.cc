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
//<ID> 1129
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
    cJSON *num_item1 = (cJSON *)0;
    cJSON *num_item2 = (cJSON *)0;
    cJSON *arr_elem0 = (cJSON *)0;
    cJSON *arr_elem1 = (cJSON *)0;
    cJSON *fetched_count = (cJSON *)0;
    cJSON *fetched_other = (cJSON *)0;
    char *printed = (char *)0;
    double v_count = 0.0;
    double v_other = 0.0;
    cJSON_bool is_count_num = (cJSON_bool)0;
    cJSON_bool is_other_num = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObjectCS(root, "items", arr);

    // step 3: Configure - add numeric members and populate array
    num_item1 = cJSON_AddNumberToObject(root, "count", 123.0);
    num_item2 = cJSON_CreateNumber(456.0);
    cJSON_AddItemToObjectCS(root, "other", num_item2);
    arr_elem0 = cJSON_CreateNumber(7.0);
    arr_elem1 = cJSON_CreateNumber(8.0);
    cJSON_AddItemToArray(arr, arr_elem0);
    cJSON_AddItemToArray(arr, arr_elem1);

    // step 4: Operate - serialize and retrieve members
    printed = cJSON_PrintUnformatted(root);
    fetched_count = cJSON_GetObjectItem(root, "count");
    fetched_other = cJSON_GetObjectItem(root, "other");
    v_count = cJSON_GetNumberValue(fetched_count);
    v_other = cJSON_GetNumberValue(fetched_other);

    // step 5: Validate - inspect types and values, free serialized buffer
    is_count_num = cJSON_IsNumber(fetched_count);
    is_other_num = cJSON_IsNumber(fetched_other);
    validation_score = (int)is_count_num + (int)is_other_num + (int)(v_count == 123.0) + (int)(v_other == 456.0);
    cJSON_free((void *)printed);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}