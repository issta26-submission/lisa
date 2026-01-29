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
//<ID> 719
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    cJSON *root = NULL;
    cJSON *meta = NULL;
    cJSON *flag_item = NULL;
    cJSON *nested_item = NULL;
    cJSON_bool flag_is_true = 0;
    double nested_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "value", 3.14);
    cJSON_AddTrueToObject(root, "flag");
    meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "nested", 42.0);

    // step 3: Operate / Validate
    flag_item = cJSON_GetObjectItem(root, "flag");
    flag_is_true = cJSON_IsTrue(flag_item);
    nested_item = cJSON_GetObjectItem(meta, "nested");
    nested_value = cJSON_GetNumberValue(nested_item);
    memcpy(scratch, &nested_value, sizeof(nested_value));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}