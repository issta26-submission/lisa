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
//<ID> 691
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    char *printbuf = (char *)cJSON_malloc(512);
    memset(printbuf, 0, 512);
    const char *json1 = "{\"name\":\"Alice\",\"ok\":true}";
    const char *json2 = "{\"greeting\":\"Hello\",\"flag\":false}";
    size_t len2 = strlen(json2);
    const char *end1 = NULL;
    const char *end2 = NULL;
    cJSON *root1 = cJSON_ParseWithOpts(json1, &end1, 1);
    cJSON *root2 = cJSON_ParseWithLengthOpts(json2, len2, &end2, 1);

    // step 2: Setup / Configure
    cJSON *combined = cJSON_CreateObject();
    cJSON_AddItemToObject(combined, "first", root1);
    cJSON_AddItemToObject(combined, "second", root2);
    cJSON_AddBoolToObject(combined, "enabled", 1);

    // step 3: Operate / Validate
    cJSON *nameItem = cJSON_GetObjectItem(root1, "name");
    cJSON_bool name_is_string = cJSON_IsString(nameItem);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(combined, printbuf, 512, 1);
    size_t out_len = strlen(printbuf);
    memcpy(scratch, printbuf, out_len);
    scratch[out_len] = '\0';

    // step 4: Cleanup
    cJSON_free(printbuf);
    cJSON_free(scratch);
    cJSON_Delete(combined);

    // API sequence test completed successfully
    return 66;
}