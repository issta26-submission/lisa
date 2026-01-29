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
//<ID> 376
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
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *optional = cJSON_AddNullToObject(meta, "optional");
    cJSON_AddTrueToObject(meta, "active");

    // step 2: Configure
    const char *name_val = cJSON_GetStringValue(name);
    double name_len = (double)strlen(name_val);
    cJSON_bool opt_is_null = cJSON_IsNull(optional);

    // step 3: Operate and Validate
    double computed_val = name_len + (double)opt_is_null * 1.0;
    cJSON *computed = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}