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
//<ID> 1534
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
    cJSON *name_item = cJSON_AddStringToObject(meta, "name", "fuzz_tester");
    cJSON *enabled_bool = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", enabled_bool);

    // step 2: Configure
    cJSON *year_num = cJSON_CreateNumber(2026.0);
    cJSON_AddItemToObject(root, "year", year_num);
    cJSON *status_str = cJSON_AddStringToObject(root, "status", "ok");

    // step 3: Operate and Validate
    double year_val = cJSON_GetNumberValue(year_num);
    const char *name_val = cJSON_GetStringValue(name_item);
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    scratch[0] = name_val ? name_val[0] : 'x';
    scratch[1] = (char)('0' + ((int)year_val % 10));
    scratch[2] = status_str && status_str->valuestring && status_str->valuestring[0] ? status_str->valuestring[0] : 's';
    scratch[3] = (char)('#');

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}