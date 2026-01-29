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
//<ID> 849
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "values", values);
    cJSON *num = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(values, num);

    // step 2: Configure
    double updated = cJSON_SetNumberHelper(num, 20.5);
    int pre_replace_size = cJSON_GetArraySize(values);
    char json_buf[] = " { \"external\" : { \"a\": 1, \"b\": 2 } } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *external = cJSON_GetObjectItem(parsed, "external");
    cJSON *external_ref = cJSON_CreateObjectReference(external);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "config", external_ref);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    double ext_a = cJSON_GetNumberValue(cJSON_GetObjectItem(external, "a"));
    (void)updated;
    (void)pre_replace_size;
    (void)ext_a;
    cJSON_free(out);

    // step 4: Validate & Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}