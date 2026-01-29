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
//<ID> 146
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *cfg = (cJSON *)0;
    cJSON *values = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *n3 = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    char *json_text = (char *)0;
    int arr_size = 0;
    cJSON_bool first_is_number = (cJSON_bool)0;
    double count_double = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    cfg = cJSON_CreateObject();
    values = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(10.0);
    n2 = cJSON_CreateNumber(20.5);
    n3 = cJSON_CreateNumber(-3.25);

    // step 3: Configure
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);
    cJSON_AddItemToObjectCS(root, "config", cfg);
    cJSON_AddItemToObject(cfg, "values", values);
    cJSON_AddTrueToObject(root, "active");

    // step 4: Operate
    arr_size = cJSON_GetArraySize(values);
    count_double = (double)arr_size;
    cJSON_AddNumberToObject(root, "count", count_double);
    first = cJSON_GetArrayItem(values, 0);
    first_is_number = cJSON_IsNumber(first);
    cJSON_AddNumberToObject(root, "first_is_number", (double)first_is_number);

    // step 5: Validate
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}