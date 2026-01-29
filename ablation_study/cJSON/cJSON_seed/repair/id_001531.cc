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
//<ID> 1531
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *enabled_flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", enabled_flag);

    // step 2: Configure
    cJSON *name_item = cJSON_AddStringToObject(settings, "name", "fuzz_tester");
    cJSON *year_item = cJSON_CreateNumber(2026.0);
    cJSON_AddItemToObject(settings, "year", year_item);

    // step 3: Operate and Validate
    const char *name_val = cJSON_GetStringValue(name_item);
    double year_val = cJSON_GetNumberValue(year_item);
    char scratch[32];
    memset(scratch, 0, sizeof(scratch));
    scratch[0] = name_val ? name_val[0] : 'x';
    scratch[1] = (char)('0' + ((int)year_val % 10));
    scratch[2] = '#';

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}