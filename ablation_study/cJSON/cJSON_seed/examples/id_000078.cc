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
//<ID> 78
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
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *greet = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greet);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(3.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double v0 = cJSON_GetNumberValue(first);
    double v1 = cJSON_GetNumberValue(second);
    cJSON *sum = cJSON_CreateNumber(v0 + v1);
    cJSON_AddItemToObject(root, "sum", sum);

    char json_buf[] = " { \"name\" : \"Alice\" , \"age\" : 30 } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *pname = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    const char *name_val = cJSON_GetStringValue(pname);
    cJSON *name_node = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "parsed_name", name_node);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}