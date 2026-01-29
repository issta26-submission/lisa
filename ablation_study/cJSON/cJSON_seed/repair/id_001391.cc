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
//<ID> 1391
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(root, "version", 2.5);
    cJSON_AddNumberToObject(meta, "threshold", 7.0);
    cJSON_AddBoolToObject(meta, "enabled", 1);

    // step 2: Configure
    cJSON *version_item = cJSON_GetObjectItemCaseSensitive(root, "version");
    double version_val = cJSON_GetNumberValue(version_item);
    cJSON *threshold_item = cJSON_GetObjectItemCaseSensitive(meta, "threshold");
    double threshold_val = cJSON_GetNumberValue(threshold_item);

    // step 3: Operate and Validate
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buf, buf_len, 0);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    char *s = (char *)scratch;
    s[0] = buf[0];
    s[1] = (char)('0' + (int)printed);
    s[2] = (char)('0' + ((int)version_val % 10));
    s[3] = (char)('0' + ((int)threshold_val % 10));

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}