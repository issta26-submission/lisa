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
//<ID> 905
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    const char *json = "[true, 42, \"hello\"]";
    cJSON_bool is_true = 0;
    double num = 0.0;
    char *buffer = NULL;
    size_t bufsize = 64;
    memset((void*)(&is_true), 0, sizeof(is_true));

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    buffer = (char *)cJSON_malloc(bufsize);
    memset(buffer, 0, bufsize);

    // step 3: Operate / Validate
    item0 = cJSON_GetArrayItem(root, 0);
    item1 = cJSON_GetArrayItem(root, 1);
    is_true = cJSON_IsTrue(item0);
    num = cJSON_GetNumberValue(item1);
    buffer[0] = (char)('0' + (int)is_true);
    buffer[1] = (char)('0' + ((int)num % 10));
    (void)buffer;
    (void)num;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}