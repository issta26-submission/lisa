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
//<ID> 1983
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *shared = cJSON_CreateObject();
    cJSON_AddNumberToObject(shared, "value", 314159.0 / 100000.0); // 3.14159
    cJSON_AddStringToObject(shared, "note", "constant_pi");
    cJSON_AddItemToObject(root, "shared_original", shared);
    cJSON *ok_flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "ok", ok_flag);

    // step 2: Configure
    cJSON *shared_ref = cJSON_CreateObjectReference((const cJSON *)shared);
    cJSON_AddItemToObject(root, "shared_ref", shared_ref);
    cJSON *status = cJSON_CreateString("ready");
    cJSON_AddItemToObject(root, "status", status);

    // step 3: Operate and Validate
    cJSON *got_ref = cJSON_GetObjectItem(root, "shared_ref");
    cJSON *got_value = cJSON_GetObjectItem(got_ref, "value");
    double num = cJSON_GetNumberValue(got_value);
    cJSON *got_note = cJSON_GetObjectItem(got_ref, "note");
    const char *note_str = cJSON_GetStringValue(got_note);
    cJSON_bool flag_is_false = cJSON_IsFalse(ok_flag);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + ((int)num % 10));
    buffer[1] = (char)('0' + (flag_is_false % 10));
    buffer[2] = (char)('0' + (printed_ok % 10));
    buffer[3] = '\0';
    (void)note_str;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}