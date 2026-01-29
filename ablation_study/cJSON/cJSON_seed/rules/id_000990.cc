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
//<ID> 990
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *values = NULL;
    cJSON *num_a = NULL;
    cJSON *num_b = NULL;
    cJSON *num_insert = NULL;
    cJSON *temp_item = NULL;
    char *printed = NULL;
    const int prebuffer = 64;
    cJSON_bool is_arr = 0;
    double first_val = 0.0;
    double second_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    values = cJSON_CreateArray();
    num_a = cJSON_CreateNumber(10.5);
    num_b = cJSON_CreateNumber(30.25);
    cJSON_AddItemToArray(values, num_a);
    cJSON_AddItemToArray(values, num_b);
    cJSON_AddItemToObject(root, "values", values);
    num_insert = cJSON_CreateNumber(20.75);
    cJSON_InsertItemInArray(values, 1, num_insert);

    // step 3: Operate / Validate
    is_arr = cJSON_IsArray(values);
    temp_item = cJSON_GetArrayItem(values, 0);
    first_val = cJSON_GetNumberValue(temp_item);
    temp_item = cJSON_GetArrayItem(values, 1);
    second_val = cJSON_GetNumberValue(temp_item);
    printed = cJSON_PrintBuffered(root, prebuffer, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}