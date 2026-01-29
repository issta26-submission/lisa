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
//<ID> 2169
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *val0 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(values, val0);

    // step 2: Configure
    cJSON *greet = cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON *strref = cJSON_CreateStringReference("ref_string");
    cJSON_AddItemToObject(root, "strref", strref);

    // step 3: Operate and Validate
    cJSON *got_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    cJSON_bool flag_is_bool = cJSON_IsBool(got_flag);
    cJSON *got_name = cJSON_GetObjectItemCaseSensitive(root, "name");
    cJSON_bool name_is_string = cJSON_IsString(got_name);
    cJSON *flag_marker = cJSON_CreateBool(flag_is_bool);
    cJSON_AddItemToObject(meta, "flag_is_bool", flag_marker);
    cJSON *name_marker = cJSON_CreateString(name_is_string ? "yes" : "no");
    cJSON_AddItemToObject(meta, "name_is_string", name_marker);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}