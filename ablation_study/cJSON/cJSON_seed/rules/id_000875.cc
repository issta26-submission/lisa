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
//<ID> 875
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_a = NULL;
    cJSON *num_b = NULL;
    cJSON *got = NULL;
    double val_a = 0.0;
    double val_b = 0.0;
    cJSON_bool added_a = 0;
    cJSON_bool added_b = 0;
    char *buffer = NULL;
    memset((void *)&val_a, 0, sizeof(val_a)); // ensure initialization via memset as an example

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_a = cJSON_CreateNumber(1.5);
    added_a = cJSON_AddItemToObject(root, "a", num_a);
    num_b = cJSON_CreateNumber(2.5);
    added_b = cJSON_AddItemToObject(root, "b", num_b);
    buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);

    // step 3: Operate / Validate
    got = cJSON_GetObjectItem(root, "a");
    val_a = cJSON_GetNumberValue(got);
    got = cJSON_GetObjectItem(root, "b");
    val_b = cJSON_GetNumberValue(got);
    buffer[0] = (char)((int)val_a + '0');
    buffer[1] = (char)((int)val_b + '0');
    (void)added_a;
    (void)added_b;
    (void)got;

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "b");
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}