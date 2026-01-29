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
//<ID> 189
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(42.5);
    cJSON *str = cJSON_CreateString("hello");
    cJSON *inner_num = cJSON_CreateNumber(7.25);
    const char *ver = cJSON_Version();
    void *buffer = cJSON_malloc(64);
    memset(buffer, 0, 64);
    double value_from_root = 0.0;
    double inner_value = 0.0;
    cJSON_bool text_is_string = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "value", num);
    cJSON_AddItemToObject(root, "text", str);
    cJSON_AddItemToObject(child, "inner", inner_num);

    // step 3: Operate / Validate
    value_from_root = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    inner_value = cJSON_GetNumberValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "child"), "inner"));
    text_is_string = cJSON_IsString(cJSON_GetObjectItem(root, "text"));
    ((char *)buffer)[0] = ver[0];
    (void)value_from_root;
    (void)inner_value;
    (void)text_is_string;
    (void)ver;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);

    // API sequence test completed successfully
    return 66;
}