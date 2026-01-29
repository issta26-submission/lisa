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
//<ID> 1248
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"a\":true,\"b\":3.14159}";
    cJSON *root = NULL;
    cJSON *item_a = NULL;
    cJSON *item_b = NULL;
    cJSON *dup_root = NULL;
    char *printed = NULL;
    char buffer[128];
    cJSON_bool prealloc_ok = 0;
    cJSON_bool is_true = 0;
    double bval = 0.0;
    double bval_dup = 0.0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    item_a = cJSON_GetObjectItem(root, "a");
    item_b = cJSON_GetObjectItem(root, "b");

    // step 3: Operate / Validate
    is_true = cJSON_IsTrue(item_a);
    bval = cJSON_GetNumberValue(item_b);
    cJSON_AddNumberToObject(root, "c", bval + 2.0);
    printed = cJSON_PrintUnformatted(root);
    memset(buffer, 0, sizeof(buffer));
    prealloc_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    dup_root = cJSON_Duplicate(root, 1);
    bval_dup = cJSON_GetNumberValue(cJSON_GetObjectItem(dup_root, "b"));
    (void)is_true;
    (void)bval;
    (void)prealloc_ok;
    (void)bval_dup;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(dup_root);
    // API sequence test completed successfully
    return 66;
}