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
//<ID> 1371
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *person = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(person, "name", name);
    cJSON_AddNumberToObject(person, "age", 30.0);
    cJSON_AddItemToObject(root, "person", person);
    cJSON *status = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "status", status);

    // step 2: Configure
    cJSON *email = cJSON_CreateString("alice@example.com");
    cJSON_AddItemToObject(person, "email", email);
    cJSON *old_note = cJSON_CreateString("initial note");
    cJSON_AddItemToObject(root, "note", old_note);

    // step 3: Operate & Validate
    cJSON_bool is_null_status = cJSON_IsNull(status);
    double status_flag = (double)is_null_status;
    cJSON_AddNumberToObject(root, "status_flag", status_flag);
    cJSON *new_note = cJSON_CreateString("updated note");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, old_note, new_note);
    double replaced_flag = (double)replaced;
    cJSON_AddNumberToObject(root, "replaced_flag", replaced_flag);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    cJSON *printed_raw = cJSON_CreateRaw(buffer);
    cJSON_AddItemToObject(root, "printed", printed_raw);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free((void *)buffer);
    // API sequence test completed successfully
    return 66;
}