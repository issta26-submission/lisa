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
//<ID> 1124
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("initial"));
    cJSON_AddItemToObject(root, "value", cJSON_CreateNumber(3.14));
    double dbls_vals[] = {1.1, 2.2, 3.3};
    cJSON *dbls = cJSON_CreateDoubleArray(dbls_vals, 3);
    cJSON_AddItemToObject(root, "dbls", dbls);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *old_name = cJSON_SetValuestring(name_item, "updated");
    cJSON_AddNumberToObject(root, "name_changed", (double)(old_name != NULL));

    // step 3: Operate
    cJSON *val_item = cJSON_GetObjectItem(root, "value");
    cJSON_bool val_is_num = cJSON_IsNumber(val_item);
    double val_num = cJSON_GetNumberValue(val_item);
    double computed = val_num + (double)val_is_num + 10.0;
    cJSON_AddNumberToObject(root, "computed", computed);

    // step 4: Validate & Cleanup
    char *unformatted = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(512);
    memset(prealloc, 0, 512);
    cJSON_PrintPreallocated(root, prealloc, 512, 1);
    cJSON_free(unformatted);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}