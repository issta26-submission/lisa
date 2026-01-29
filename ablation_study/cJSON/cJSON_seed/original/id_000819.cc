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
//<ID> 819
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *info = cJSON_AddObjectToObject(root, "info");
    cJSON *pi_item = cJSON_CreateNumber(0.0);

    // step 2: Configure
    cJSON_AddStringToObject(meta, "version", cJSON_Version());
    cJSON_SetNumberHelper(pi_item, 3.141592653589793);
    cJSON_AddItemToObject(info, "pi", pi_item);
    cJSON_AddStringToObject(info, "status", "ok");

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    cJSON_free(pretty);
    cJSON_free(compact);

    // step 4: Validate & Cleanup
    cJSON *got_version = cJSON_GetObjectItem(meta, "version");
    const char *verstr = cJSON_GetStringValue(got_version);
    cJSON *got_pi = cJSON_GetObjectItem(info, "pi");
    double pi_val = cJSON_GetNumberValue(got_pi);
    (void)verstr;
    (void)pi_val;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}