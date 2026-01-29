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
//<ID> 1386
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddNumberToObject(root, "a", 10.5);
    cJSON_AddNumberToObject(root, "b", 20.25);

    // step 3: Operate and Validate
    cJSON *item_a = cJSON_GetObjectItem(root, "a");
    double a_val = cJSON_GetNumberValue(item_a);
    cJSON *item_b = cJSON_GetObjectItem(root, "b");
    double b_val = cJSON_GetNumberValue(item_b);
    double total = a_val + b_val;
    cJSON_AddNumberToObject(root, "total", total);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    char *s = (char *)scratch;
    s[0] = buffer[0];
    s[1] = (char)('0' + (int)total % 10);
    s[2] = (char)('0' + (int)ok);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}