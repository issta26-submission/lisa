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
//<ID> 466
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"id\":7,\"name\":\"fuzz\"}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *id_item = cJSON_GetObjectItem(root, "id");
    double orig_id = cJSON_GetNumberValue(id_item);

    // step 2: Configure
    double computed = orig_id * 3.14;
    cJSON *computed_num = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_num);
    cJSON *marker = cJSON_AddNullToObject(root, "marker");

    // step 3: Operate
    cJSON *retrieved = cJSON_GetObjectItem(root, "computed");
    double retrieved_val = cJSON_GetNumberValue(retrieved);
    char *snapshot = cJSON_PrintUnformatted(root);
    char tmpbuf[32];
    memset(tmpbuf, 0, sizeof(tmpbuf));
    (void)retrieved_val;
    (void)marker;
    (void)tmpbuf;

    // step 4: Validate & Cleanup
    cJSON *check_marker = cJSON_GetObjectItem(root, "marker");
    cJSON_bool is_null = cJSON_IsNull(check_marker);
    (void)is_null;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}