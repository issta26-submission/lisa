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
//<ID> 76
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

    // step 2: Configure
    cJSON *name = cJSON_CreateString("example");
    cJSON *value = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObject(meta, "value", value);

    // step 3: Operate & Validate
    double v = cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "value"));
    const char *s = cJSON_GetStringValue(cJSON_GetObjectItem(meta, "name"));
    cJSON *doubled = cJSON_CreateNumber(v * 2.0);
    cJSON_AddItemToObject(root, "double_value", doubled);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}