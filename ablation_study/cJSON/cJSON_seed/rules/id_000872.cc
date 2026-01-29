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
//<ID> 872
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_one = NULL;
    cJSON *num_two = NULL;
    cJSON *retrieved = NULL;
    char *printed = NULL;
    void *buf = NULL;
    size_t buf_size = 128;
    double pi_value = 0.0;
    cJSON_bool added_one = 0;
    cJSON_bool added_two = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_one = cJSON_CreateNumber(3.14159);
    added_one = cJSON_AddItemToObject(root, "pi", num_one);
    num_two = cJSON_CreateNumber(100.0);
    added_two = cJSON_AddItemToObject(root, "temp", num_two);
    buf = cJSON_malloc(buf_size);
    memset(buf, 0, buf_size);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "pi");
    pi_value = cJSON_GetNumberValue(retrieved);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "temp");
    printed = cJSON_PrintUnformatted(root);
    ((char *)buf)[0] = (char)pi_value;
    ((char *)buf)[1] = (char)(added_one + added_two);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}