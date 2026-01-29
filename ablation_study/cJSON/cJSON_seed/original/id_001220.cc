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
//<ID> 1220
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
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *str1 = cJSON_CreateString("hello world");
    cJSON_AddItemToArray(arr, str1);
    cJSON *str2 = cJSON_CreateString("another entry");
    cJSON_AddItemToArray(arr, str2);
    const char *ver = cJSON_Version();
    cJSON_AddStringToObject(root, "version", ver);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed) + 1;
    char *buf = (char *)cJSON_malloc(printed_len);
    memset(buf, 0, printed_len);
    memcpy(buf, printed, printed_len - 1);
    cJSON_Minify(buf);
    cJSON *parsed = cJSON_Parse(buf);
    cJSON_bool parsed_is_array = cJSON_IsArray(parsed);
    cJSON *flag = cJSON_CreateBool(parsed_is_array);
    cJSON_AddItemToObject(root, "parsed_is_array", flag);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}