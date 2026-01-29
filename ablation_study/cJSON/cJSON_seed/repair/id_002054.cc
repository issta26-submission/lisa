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
//<ID> 2054
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(array, num);
    cJSON *str0 = cJSON_CreateString("original");
    cJSON_AddItemToArray(array, str0);

    // step 2: Configure
    char json_buf[] = " { \"minified\" : \"yes\" } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *min_item = cJSON_GetObjectItem(parsed, "minified");
    const char *minstr = cJSON_GetStringValue(min_item);
    cJSON *from_parsed = cJSON_CreateString(minstr);
    cJSON_AddItemToArray(array, from_parsed);
    cJSON *dup_array = cJSON_Duplicate(array, (cJSON_bool)1);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = cJSON_GetStringValue(cJSON_GetArrayItem(dup_array, 1))[0];
    buffer[2] = minstr[0];
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(dup_array);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}