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
//<ID> 595
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, n1);
    cJSON *s1 = cJSON_CreateString("value");
    cJSON_AddItemToArray(items, s1);
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddItemToArray(items, obj);
    cJSON *inner = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(obj, "inner", inner);
    cJSON *b = cJSON_AddBoolToObject(obj, "flag", 1);

    // step 2: Configure
    int count = cJSON_GetArraySize(items);
    double scaling = (double)count + cJSON_GetNumberValue(n1) / 10.0;
    cJSON_AddNumberToObject(root, "scale", scaling);

    // step 3: Operate and Validate
    const char *str = cJSON_GetStringValue(s1);
    double innerv = cJSON_GetNumberValue(inner);
    cJSON_bool is_flag_true = cJSON_IsTrue(b);
    double derived = innerv * (1.0 + (double)is_flag_true);
    cJSON_AddNumberToObject(root, "derived", derived);
    cJSON_AddStringToObject(root, "first_string", str);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}