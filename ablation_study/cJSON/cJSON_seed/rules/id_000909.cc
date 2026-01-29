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
//<ID> 909
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *flags = NULL;
    cJSON *flag0 = NULL;
    cJSON *flag1 = NULL;
    cJSON *flag2 = NULL;
    cJSON_bool is0 = 0;
    cJSON_bool is1 = 0;
    cJSON_bool is2 = 0;
    double value = 0.0;
    int count = 0;
    int sum_flags = 0;
    char *buffer = NULL;
    const char *json = "{\"flags\":[true,false,true],\"value\":123.5}";

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    flags = cJSON_GetObjectItem(root, "flags");
    count = cJSON_GetArraySize(flags);
    flag0 = cJSON_GetArrayItem(flags, 0);
    flag1 = cJSON_GetArrayItem(flags, 1);
    flag2 = cJSON_GetArrayItem(flags, 2);

    // step 3: Operate / Validate
    is0 = cJSON_IsTrue(flag0);
    is1 = cJSON_IsTrue(flag1);
    is2 = cJSON_IsTrue(flag2);
    sum_flags = (int)(is0 + is1 + is2);
    value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = (char)('0' + (sum_flags % 10));
    buffer[1] = (char)('0' + (((int)value) % 10));
    (void)count;
    (void)value;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}