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
//<ID> 1397
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "count", 2024.0);
    cJSON *active_item = cJSON_AddBoolToObject(root, "active", 1);

    // step 2: Configure
    const cJSON *count_item = cJSON_GetObjectItemCaseSensitive(root, "count");
    const cJSON *active_lookup = cJSON_GetObjectItemCaseSensitive(root, "active");
    double count_val = cJSON_GetNumberValue(count_item);
    int active_flag_lookup = active_lookup->valueint;

    // step 3: Operate and Validate
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buf, buf_len, 0);
    void *scratch = cJSON_malloc(16);
    memset(scratch, 0, 16);
    char *s = (char *)scratch;
    s[0] = buf[0];
    s[1] = (char)('0' + ((int)count_val % 10));
    s[2] = (char)('0' + active_flag_lookup);
    s[3] = (char)('0' + active_item->valueint);

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}