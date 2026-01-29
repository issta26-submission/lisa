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
//<ID> 1147
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char json1[] = "{\"vals\":[10,20,30]}";
    const char json2[] = "{\"flag\":true}";
    cJSON *parsed = cJSON_Parse(json1);
    cJSON *parsed2 = cJSON_ParseWithLength(json2, sizeof(json2) - 1);
    cJSON *nul = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObject(root, "primary", parsed);
    cJSON_AddItemToObject(root, "secondary", parsed2);
    cJSON_AddItemToObject(root, "empty", nul);
    cJSON *vals = cJSON_GetObjectItem(parsed, "vals");
    cJSON *extra = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(vals, extra);

    // step 3: Operate & Validate
    cJSON *second_item = cJSON_GetArrayItem(vals, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    cJSON_AddNumberToObject(root, "second_value_plus_one", second_value + 1.0);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}