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
//<ID> 213
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(42.0);
    double before_value = 0.0;
    double after_value = 0.0;
    double set_ret = 0.0;
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed_ok = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "answer", num);
    before_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "answer"));

    // step 3: Operate / Validate
    set_ret = cJSON_SetNumberHelper(num, 3.14159);
    after_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "answer"));
    printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    (void)before_value;
    (void)set_ret;
    (void)after_value;
    (void)printed_ok;
    (void)buffer[0];

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}