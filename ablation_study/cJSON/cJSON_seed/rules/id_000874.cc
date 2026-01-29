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
//<ID> 874
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *n2b = NULL;
    cJSON *got = NULL;
    char *name1 = NULL;
    char *name2 = NULL;
    double extracted = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name1 = (char *)cJSON_malloc(16);
    name2 = (char *)cJSON_malloc(16);
    memset(name1, 0, 16);
    memset(name2, 0, 16);
    name1[0] = 'n'; name1[1] = 'u'; name1[2] = 'm'; name1[3] = '1'; name1[4] = '\0';
    name2[0] = 'n'; name2[1] = 'u'; name2[2] = 'm'; name2[3] = '2'; name2[4] = '\0';
    n1 = cJSON_CreateNumber(123.0);
    n2 = cJSON_CreateNumber(456.0);
    cJSON_AddItemToObject(root, name1, n1);
    cJSON_AddItemToObject(root, name2, n2);

    // step 3: Operate / Validate
    got = cJSON_GetObjectItem(root, name1);
    extracted = cJSON_GetNumberValue(got);
    cJSON_DeleteItemFromObjectCaseSensitive(root, name2);
    n2 = NULL;
    n2b = cJSON_CreateNumber(789.0);
    cJSON_AddItemToObject(root, name2, n2b);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(name1);
    cJSON_free(name2);
    // API sequence test completed successfully
    (void)got;
    (void)extracted;
    return 66;
}