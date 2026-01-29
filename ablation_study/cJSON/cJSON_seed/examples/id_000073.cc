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
//<ID> 73
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
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToArray(values, num2);
    char json_buf[] = " { \"name\" : \"example\", \"count\" : 7 } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *parsed_name = cJSON_GetObjectItem(parsed, "name");
    cJSON *parsed_count = cJSON_GetObjectItem(parsed, "count");
    const char *name_val = cJSON_GetStringValue(parsed_name);
    double count_val = cJSON_GetNumberValue(parsed_count);
    cJSON *name_str = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "parsed_name", name_str);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(values, 0);
    cJSON *second = cJSON_GetArrayItem(values, 1);
    double v0 = cJSON_GetNumberValue(first);
    double v1 = cJSON_GetNumberValue(second);
    cJSON *sum = cJSON_CreateNumber(v0 + v1 + count_val);
    cJSON_AddItemToObject(root, "sum", sum);
    cJSON *count_num = cJSON_CreateNumber(count_val);
    cJSON_AddItemToObject(root, "parsed_count", count_num);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}