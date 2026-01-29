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
//<ID> 476
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"id\":10,\"name\":\"test\"}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *id_item = cJSON_GetObjectItem(root, "id");
    double id_val = cJSON_GetNumberValue(id_item);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "valid");
    cJSON *refs = cJSON_AddArrayToObject(root, "references");
    cJSON *orig = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "original", orig);

    // step 3: Operate
    cJSON_bool is_obj = cJSON_IsObject(root);
    double computed = id_val + cJSON_GetNumberValue(orig) + (double)is_obj;
    double set_back = cJSON_SetNumberHelper(orig, computed);
    cJSON_bool ref_added = cJSON_AddItemReferenceToArray(refs, orig);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)set_back;
    (void)ref_added;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}