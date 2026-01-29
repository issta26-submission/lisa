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
//<ID> 372
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *text = "hello_world";
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString(text);
    cJSON_AddItemToObject(root, "name", name);
    cJSON *num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", num);
    cJSON *maybe = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe);

    // step 2: Configure
    cJSON *name_copy = cJSON_Duplicate(name, 0);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON *dup_root = cJSON_Duplicate(root, 1);

    // step 3: Operate & Validate
    double pi_val = cJSON_GetNumberValue(num);
    cJSON_bool was_null = cJSON_IsNull(maybe);
    char status_buf[64];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "pi=%.5f|null=%d", pi_val, (int)was_null);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);
    cJSON *dup_pi = cJSON_GetObjectItem(dup_root, "pi");
    double dup_pi_val = cJSON_GetNumberValue(dup_pi);
    char dupinfo_buf[64];
    memset(dupinfo_buf, 0, sizeof(dupinfo_buf));
    sprintf(dupinfo_buf, "dup_pi=%.5f", dup_pi_val);
    cJSON *dupinfo = cJSON_CreateString(dupinfo_buf);
    cJSON_AddItemToObject(root, "dupinfo", dupinfo);
    char *snapshot = cJSON_PrintUnformatted(dup_root);
    cJSON *snapstr = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapstr);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}