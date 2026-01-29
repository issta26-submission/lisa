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
//<ID> 356
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n3 = cJSON_CreateNumber(30.25);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n3);
    cJSON *n2 = cJSON_CreateNumber(20.75);
    cJSON_InsertItemInArray(values, 1, n2);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);
    char jsonbuf[] = " { \"a\" : 1 , \"b\" : 2 } ";
    cJSON_Minify(jsonbuf);

    // step 2: Configure
    cJSON *first = cJSON_GetArrayItem(values, 0);
    double v0 = cJSON_GetNumberValue(first);
    cJSON *second = cJSON_GetArrayItem(values, 1);
    double v1 = cJSON_GetNumberValue(second);
    cJSON *third = cJSON_GetArrayItem(values, 2);
    double v2 = cJSON_GetNumberValue(third);
    double sum = v0 + v1 + v2;
    cJSON_AddNumberToObject(root, "sum", sum);

    // step 3: Operate and Validate
    cJSON_bool is_flag_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "flag"));
    double extra = (double)(!is_flag_false) * 5.0;
    cJSON_AddNumberToObject(root, "adjusted_sum", sum + extra);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}