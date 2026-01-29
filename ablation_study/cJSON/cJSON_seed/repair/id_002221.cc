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
//<ID> 2221
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *obj1 = cJSON_CreateObject();
    cJSON *obj2 = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *str = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "objA", obj1);
    cJSON_AddItemToObject(root, "objB", obj2);
    cJSON_AddItemToObject(root, "answer", num);
    cJSON_AddItemToObject(root, "greet", str);

    // step 2: Configure
    cJSON_AddItemToObject(obj1, "inner_str", cJSON_CreateStringReference("inner"));
    cJSON_AddItemToObject(obj1, "inner_num", cJSON_CreateNumber(7.0));
    cJSON_AddItemToObject(obj2, "flag", cJSON_CreateFalse());
    cJSON_AddItemToObject(obj2, "nested", cJSON_CreateObject());
    cJSON *nested = cJSON_GetObjectItem(obj2, "nested");
    cJSON_AddItemToObject(nested, "deep", cJSON_CreateString("deep_value"));

    // step 3: Operate and Validate
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_bool obj2_is_obj = cJSON_IsObject(obj2);
    cJSON *got_answer = cJSON_GetObjectItem(root, "answer");
    double ans_val = cJSON_GetNumberValue(got_answer);
    cJSON *got_greet = cJSON_GetObjectItem(root, "greet");
    char *greet_val = cJSON_GetStringValue(got_greet);
    (void)greet_val;
    (void)root_is_obj;
    (void)obj2_is_obj;
    (void)ans_val;
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "objA");
    cJSON_DeleteItemFromObject(root, "answer");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}