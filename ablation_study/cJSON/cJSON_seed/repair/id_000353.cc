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
//<ID> 353
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *n2 = cJSON_CreateNumber(7.0);
    cJSON *f = cJSON_CreateFalse();
    char json_buf[] = " { \"x\": 1, \"y\": 2 } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_InsertItemInArray(arr, 0, n2);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "flag", f);
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON_AddNumberToObject(root, "sum", cJSON_GetNumberValue(n1) + cJSON_GetNumberValue(n2));

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double val0 = cJSON_GetNumberValue(first);
    cJSON_bool is_false = cJSON_IsFalse(f);
    double computed = val0 + (double)is_false * 100.0;
    cJSON *res = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "result", res);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}