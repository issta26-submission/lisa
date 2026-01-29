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
//<ID> 597
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(3.14159);
    cJSON *num2 = cJSON_CreateNumber(42.0);
    size_t printed_len = 0;
    size_t to_copy = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddStringToObject(root, "pi_str", "3.14159");
    cJSON_AddItemToObject(root, "numbers", array);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 255 ? printed_len : 255;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}