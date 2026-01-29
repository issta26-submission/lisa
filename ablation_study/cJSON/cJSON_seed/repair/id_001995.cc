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
//<ID> 1995
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *orig_name = cJSON_CreateString("original_name");
    cJSON *count = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(child, "name", orig_name);
    cJSON_AddItemToObject(child, "count", count);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *label = cJSON_CreateString("initial_label");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    cJSON_AddFalseToObject(root, "enabled");
    cJSON_AddNullToObject(root, "optional");
    const char *ver = cJSON_Version();
    cJSON_AddStringToObject(root, "version", ver);
    char *setres = cJSON_SetValuestring(label, "updated_label");

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 0);
    buffer[0] = (char)('0' + (printed_ok % 10));
    buffer[1] = (setres && setres[0]) ? setres[0] : 'x';
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}