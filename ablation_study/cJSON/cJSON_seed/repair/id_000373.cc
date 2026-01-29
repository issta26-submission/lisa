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
//<ID> 373
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
    cJSON *label = cJSON_CreateString("hello");
    cJSON_AddItemToObject(meta, "label", label);
    cJSON_AddTrueToObject(root, "active");

    // step 2: Configure
    cJSON *optional = cJSON_CreateNull();
    cJSON_AddItemToObject(meta, "optional", optional);
    cJSON *note = cJSON_CreateString("sample-note");
    cJSON_AddItemToObject(root, "note", note);

    // step 3: Operate and Validate
    const char *sval = cJSON_GetStringValue(label);
    double computed = (double)sval[0] + (double)cJSON_IsNull(optional);
    cJSON_AddNumberToObject(root, "computed", computed);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}