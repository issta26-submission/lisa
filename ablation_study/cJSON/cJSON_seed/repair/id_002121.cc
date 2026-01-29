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
//<ID> 2121
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(array, s1);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(array, s2);
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(array, n1);
    cJSON *num_obj = cJSON_AddNumberToObject(root, "count", 2.0);

    // step 3: Operate and Validate
    cJSON *found_num = cJSON_GetObjectItemCaseSensitive(root, "count");
    double numval = cJSON_GetNumberValue(found_num);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (char)('0' + ((int)numval % 10));
    cJSON *greet = cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON *found_greet = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    const char *gstr = cJSON_GetStringValue(found_greet);
    (void)gstr;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}