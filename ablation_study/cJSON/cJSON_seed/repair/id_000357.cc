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
//<ID> 357
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
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_InsertItemInArray(arr, 0, n1);
    cJSON_InsertItemInArray(arr, 1, n2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "init_sum", 30.0);

    // step 2: Configure
    char jsonbuf[] = "{ \"a\": 1, \"b\": false, \"arr\": [ 1, 2, 3 ] }";
    cJSON_Minify(jsonbuf);
    cJSON *parsed = cJSON_Parse(jsonbuf);
    cJSON *b_item = cJSON_GetObjectItem(parsed, "b");
    cJSON_bool b_is_false = cJSON_IsFalse(b_item);
    cJSON_AddNumberToObject(root, "b_is_false", (double)b_is_false);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double v_first = cJSON_GetNumberValue(first);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double v_second = cJSON_GetNumberValue(second);
    double computed = v_first + v_second + (double)b_is_false;
    cJSON_AddNumberToObject(root, "computed_total", computed);
    cJSON *inserted = cJSON_CreateNumber(5.0);
    cJSON_InsertItemInArray(arr, 1, inserted);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}