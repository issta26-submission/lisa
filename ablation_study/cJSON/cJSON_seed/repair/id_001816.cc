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
//<ID> 1816
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"initial\",\"meta\":{\"count\":1}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");

    // step 2: Configure
    cJSON_SetValuestring(name_item, "updated");
    cJSON_AddNumberToObject(root, "version_code", 2026.0);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON_AddNumberToObject(meta, "updated_ts", 1618033.0);

    // step 3: Operate and Validate
    const char *ver = cJSON_Version();
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = ver[0];
    const char *updated_name = cJSON_GetStringValue(name_item);
    double version_code = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "version_code"));
    (void)updated_name; (void)version_code; (void)parse_end;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}