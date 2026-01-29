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
//<ID> 878
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
    cJSON *num_c = NULL;
    cJSON *found = NULL;
    char *mem = NULL;
    char buffer[8];
    double val_a = 0.0;
    cJSON_bool ok1 = 0;
    cJSON_bool ok2 = 0;
    cJSON_bool ok3 = 0;
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_a = cJSON_CreateNumber(123.5);
    ok1 = cJSON_AddItemToObject(root, "a", num_a);
    num_b = cJSON_CreateNumber(456.25);
    ok2 = cJSON_AddItemToObject(root, "b", num_b);

    // step 3: Operate / Validate
    found = cJSON_GetObjectItem(root, "a");
    val_a = cJSON_GetNumberValue(found);
    num_c = cJSON_CreateNumber(val_a * 2.0);
    ok3 = cJSON_AddItemToObject(root, "c", num_c);
    mem = (char *)cJSON_malloc(4);
    mem[0] = 'O';
    mem[1] = 'K';
    mem[2] = '\0';
    buffer[0] = (char)('0' + (ok1 + ok2 + ok3));
    cJSON_DeleteItemFromObjectCaseSensitive(root, "b");

    // step 4: Cleanup
    cJSON_free(mem);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)buffer;
    (void)val_a;
    (void)found;
    return 66;
}